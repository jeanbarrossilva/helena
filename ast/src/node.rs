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

use std::{
  any::{Any, TypeId},
  error::Error,
  fmt::{Debug, Display, Formatter}
};

use regex::Regex;

#[cfg(windows)]
const NEWLINE: &str = "\r\n";

#[cfg(not(windows))]
const NEWLINE: &str = "\n";

/// Error that occurs when the name of a node that is required to follow a specific pattern does not
/// match it.
#[derive(Debug)]
struct TextOutOfPatternError<'a> {
  /// The pattern out of which the text is.
  pattern: Regex,

  /// The text that does not match the pattern specified by the node.
  text: &'a str
}

impl Display for TextOutOfPatternError<'_> {
  fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
    Display::fmt(
      &format!("\"{}\" does not match \"{}\".", self.text, self.pattern),
      f
    )
  }
}

impl Error for TextOutOfPatternError<'_> {}

pub(crate) trait Text<'a> {
  fn as_string() -> Result<&'a str, TextOutOfPatternError>;
}

/// A token in the AST.
pub trait Node: Debug {
  /// Obtains the vertical position of this node in the AST. Corresponds to the number of the line
  /// in which it is located in the source file.
  fn column(&self) -> u32;

  /// Obtains the horizontal position of this node in the AST. Corresponds to the index of the
  /// character that delimits the start of this node in the line of the source file in which it is
  /// located.
  fn row(&self) -> u32;

  /// Obtains the textual representation of this node in the AST.
  fn text(&self) -> Result<&str, TextOutOfPatternError>;

  /// Obtains the nodes that can succeed this one in the AST. [None] delimits the end of the lexeme.
  fn next(&self) -> &Vec<Option<Self>>;
}

/// Node of a user-defined name for a structure which has either being declared or has already been.
#[derive(Debug)]
pub(crate) struct IdentifierNode<'a, N>
where
  N: Node
{
  /// Textual representation of this node in the AST.
  text: &'a str,

  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32,

  /// Nodes that can succeed this one in the tree.
  next: Vec<Option<N>>
}

impl<N> IdentifierNode<N>
where
  N: Node
{
  /// Instantiates an identifier node.
  pub(crate) fn new(text: &str, column: u32, row: u32, next: Vec<Option<N>>) -> Self {
    IdentifierNode {
      text,
      column,
      row,
      next
    }
  }
}

impl<'a, N> Node for IdentifierNode<'a, N> {
  fn column(&self) -> u32 {
    self.column
  }

  fn row(&self) -> u32 {
    self.row
  }

  fn text(&self) -> Result<&str, TextOutOfPatternError> {
    if !Regex::new(r"[a-zA-Z0-9_]*").unwrap().is_match(self.text) {
      Err()
    }
  }

  fn next(&self) -> &Vec<Option<N>> {
    &self.next
  }
}

impl NonStrictNode for IdentifierNode<_> {
  fn name(&self) -> Result<&str, impl Error> {
    if !self.pattern().is_match(self.text) {
      return;
    }
  }

  fn pattern(&self) -> Regex {}
}

#[derive(Debug)]
pub(crate) struct SpacingNode<N>
where
  N: Node
{
  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32,

  /// Nodes that can succeed this one in the tree.
  pub(crate) next: Vec<Option<N>>
}

impl<N> Node for SpacingNode<N>
where
  N: Node
{
  fn column(&self) -> u32 {
    self.column
  }

  fn row(&self) -> u32 {
    self.row
  }

  fn next(&self) -> &Vec<Option<N>> {
    &self.next
  }
}

impl NonStrictNode for SpacingNode<_> {
  fn pattern(&self) -> &str {
    " "
  }
}

/// Node of either a statement or an expression.
#[derive(Debug)]
pub(crate) struct OperationNode<'a, N>
where
  N: Node
{
  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32,

  /// Nodes that can succeed this one in the tree.
  pub(crate) next: Vec<Option<N>>
}

impl<N> Node for OperationNode<N>
where
  N: Node
{
  fn column(&self) -> u32 {
    self.column
  }

  fn row(&self) -> u32 {
    self.row
  }

  fn next(&self) -> &Vec<Option<N>> {
    &self.next
  }
}

impl NonStrictNode for OperationNode<_> {
  fn name(&self) -> Result<&str, Error> {}

  fn pattern(&self) -> Regex {
    Regex::new(r"\w*").unwrap()
  }
}

#[derive(Debug)]
pub(crate) struct ListSeparatorNode<N>
where
  N: Node
{
  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32,

  /// Nodes that can succeed this one in the tree.
  pub(crate) next: Vec<Option<N>>
}

impl<N> Node for ListSeparatorNode<N>
where
  N: Node
{
  fn column(&self) -> u32 {
    self.column
  }

  fn row(&self) -> u32 {
    self.row
  }

  fn next(&self) -> &Vec<Option<N>> {
    &self.next
  }
}

impl StrictNode for ListSeparatorNode<_> {
  fn name(&self) -> &str {
    ", "
  }
}

/// Node of an OS-specific linebreak.
#[derive(Debug)]
pub(crate) struct NewlineNode<N>
where
  N: Node
{
  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32,

  /// Nodes that can succeed this one in the tree.
  next: Vec<Option<N>>
}

impl<N> Node for NewlineNode<N>
where
  N: Node
{
  fn column(&self) -> u32 {
    self.column
  }

  fn row(&self) -> u32 {
    self.row
  }

  fn next(&self) -> &Vec<Option<N>> {
    &self.next
  }
}

impl StrictNode for NewlineNode<_> {
  fn name(&self) -> &str {
    NEWLINE
  }
}
