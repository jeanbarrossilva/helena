// Copyright © 2025 Jean Silva
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
//                            http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied. See the License for the specific language governing permissions and limitations under
// the License.

use std::fmt::{Display, Formatter};

use regex::Regex;

/// Error that occurs when the name of a node that is required to follow a specific pattern does not
/// match it.
#[derive(Debug)]
pub struct UnmatchedPatternError {
  /// The message to be displayed that describes the error.
  message: String
}

impl Display for UnmatchedPatternError {
  fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
    Display::fmt(&self.message, f)
  }
}

impl std::error::Error for UnmatchedPatternError {}

/// A node (token) in the AST.
#[derive(Debug, PartialEq)]
pub struct Node<'a> {
  /// Textual representation of this node in the AST.
  text: &'a str,

  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32,

  /// Expected next nodes; a [None] being present in this [Vec] means that this can be a leaf node,
  /// while [Some]s hold the nodes by which this one can be followed. Nothing prevents both
  /// scenarios from being true: there might be the expectation for this node to be a leaf node,
  /// but, in case it is not, for there to be other specific nodes that succeed it.
  ///
  /// ```rust
  /// use ast::node::Node;
  ///
  /// // Here, "a" can either be a leaf node — be the final one of the rule — or be followed by "b".
  /// Node::new("a", 0, 0).expect("b", |node| Ok(node)).and_then(|node| node.leaf()).unwrap();
  /// ```
  branches: Vec<Option<Node<'a>>>
}

impl<'a> Node<'a> {
  /// Instantiates a node.
  pub fn new(text: &'a str, column: u32, row: u32) -> Self {
    Node {
      text,
      column,
      row,
      branches: Vec::new()
    }
  }
}

impl<'a> Node<'a> {
  /// Denotes that this node can be the last one — a leaf — of the lexeme.
  pub fn leaf(mut self) -> Result<Self, UnmatchedPatternError> {
    if self.branches.contains(&None) {
      return Ok(self);
    }
    self.branches.push(None);
    Ok(self)
  }

  /// Denotes that a node whose text matches the given pattern can be the next one.
  pub(crate) fn expect_with_pattern<
    E: Fn() -> String,
    C: Fn(Self) -> Result<Self, UnmatchedPatternError>
  >(
    self,
    pattern: Regex,
    error_message: E,
    text: &'a str,
    chain: C
  ) -> Result<Self, UnmatchedPatternError> {
    if !pattern.is_match(text) {
      return Err(UnmatchedPatternError {
        message: error_message()
      });
    }
    self.expect(text, chain)
  }

  /// Denotes that a node with the given textual representation can be the next one.
  pub fn expect<F: Fn(Self) -> Result<Self, UnmatchedPatternError>>(
    mut self,
    text: &'a str,
    chain: F
  ) -> Result<Self, UnmatchedPatternError> {
    let next = Self::new(text, self.next_column(text), self.next_row(text));
    self.branches.push(chain(next).ok());
    Ok(self)
  }

  /// Obtains the column in which the node that follows this one is expected to be.
  fn next_column(&'a self, text: &'a str) -> u32 {
    if self.column > 0 && self.next_row(text) == 0 {
      return self.column + 1;
    }
    self.column
  }

  /// Obtains the row in which the node that follows this one is expected to be.
  fn next_row(&'a self, text: &'a str) -> u32 {
    (self.row as usize + text.len()).min(100) as u32
  }
}

impl Display for Node<'_> {
  fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
    write!(f, "{}\n", self.text)
  }
}
