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

use std::sync::OnceLock;
use regex::Regex;

use crate::{
  branch::Branch,
  common::{identifier::IdentifierNode, operation::OperationNode, spacing::SpacingNode},
  node::{Node, UnmatchedPatternError}
};

#[derive(Debug)]
pub(crate) struct ValueParameter<'a> {
  /// Name of the type of the value as it was input by the user: may or may not be qualified.
  pub(crate) type_name: &'a str,

  /// Name attributed to this value parameter.
  pub(crate) identifier: &'a str
}

/// Node of a declaration of a function.
#[derive(Debug)]
pub(crate) struct FunctionNode<'a> {
  /// Identifier of the function.
  identifier: &'a str,

  /// Values expected to be passed in to the function as parameters.
  value_parameters: Vec<ValueParameter<'a>>,

  /// Vertical position of this node in the AST. Corresponds to the number of the line in which it
  /// is located in the source file.
  column: u32,

  /// Horizontal position of this node in the AST. Corresponds to the index of the character that
  /// delimits the start of this node in the line of the source file in which it is located.
  row: u32
}

impl<'a> Node for FunctionNode<'a> {
  fn column(&'a self) -> u32 {
    self.column
  }

  fn row(&'a self) -> u32 {
    self.row
  }

  fn text(&'a self) -> Result<&'a str, UnmatchedPatternError> {
    Ok("func")
  }

  fn branch(&'a self) -> &'a Branch<'a, IdentifierNode<'a, impl Node>> {
    static BRANCH: OnceLock<Branch<'a>> = OnceLock::new();
    BRANCH.get_or_init(|| Branch::new("func", self.column(), self.row()));
    vec![Some(IdentifierNode::new(
      self.identifier,
      self.column,
      self.row,
      vec![Some(ValueParameterListDeclarationStartNode {})]
    ))]
  }
}
