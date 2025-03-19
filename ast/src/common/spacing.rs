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

use crate::{
  branch::Branch,
  node::{Node, UnmatchedPatternError}
};

#[derive(Debug)]
pub(crate) struct SpacingNode<'a, N: Node> {
  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32,

  /// Branch by which the nodes that are expected to follow this one are defined.
  branch: &'a Branch<'a, N>
}

impl<'a, N: Node> SpacingNode<'a, N> {
  // Textual representation of a spacing node in the AST.
  const TEXT: Result<&'a str, UnmatchedPatternError<'a>> = Ok(" ");

  /// Instantiates a spacing node.
  fn new(column: u32, row: u32, branch: &Branch<N>) -> Self {
    SpacingNode {
      column,
      row,
      branch
    }
  }
}

impl<N: Node> Node for SpacingNode<'_, N> {
  fn column(&self) -> u32 {
    self.column
  }

  fn row(&self) -> u32 {
    self.row
  }

  fn text(&self) -> Result<&str, UnmatchedPatternError> {
    Self::TEXT
  }

  fn branch(&self) -> &Branch<N> {
    self.branch
  }
}

impl<N1: Node> Branch<'_, SpacingNode<'_, N1>> {
  /// Denotes that a spacing node is expected to follow the current node.
  pub(crate) fn spacing<N2: Node, F: Fn(&SpacingNode<'_, N1>) -> &Branch<'_, N2>>(
    &mut self,
    current: &impl Node,
    chain: F
  ) -> &Self {
    let next = SpacingNode::new(
      self.column_of(current, SpacingNode::TEXT.unwrap()),
      self.row_of(current, SpacingNode::TEXT.unwrap()),
      &self
    );
    self.expect(next.clone(), chain)
  }
}
